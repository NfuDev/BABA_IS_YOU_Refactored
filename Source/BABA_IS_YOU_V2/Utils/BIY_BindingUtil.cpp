// Demo Project Developed By Nightfall16 @2024 , Clone To The Real 'Baba Is You' Game.



#include "BIY_BindingUtil.h"
#include "Engine/InputActionDelegateBinding.h"
#include "Engine/InputAxisDelegateBinding.h"
#include "Engine/InputKeyDelegateBinding.h"
#include "Engine/InputTouchDelegateBinding.h"
#include "Engine/InputAxisKeyDelegateBinding.h"
#include "Engine/InputVectorAxisDelegateBinding.h"

void UBIY_BindingUtil::BindInputDelegatesToObject(const UClass* InClass, UInputComponent* InputComponent, UObject* GraphObject)
{
    static TArray<UClass*> InputBindingClasses = {
       UInputActionDelegateBinding::StaticClass(),
       UInputAxisDelegateBinding::StaticClass(),
       UInputKeyDelegateBinding::StaticClass(),
       UInputTouchDelegateBinding::StaticClass(),
       UInputAxisKeyDelegateBinding::StaticClass(),
       UInputVectorAxisDelegateBinding::StaticClass(),
    };

    if (InClass)
    {
        // Bind parent class input delegates
        BindInputDelegatesToObject(InClass->GetSuperClass(), InputComponent, GraphObject);

        // Bind own graph input delegates
        for (int32 Index = 0; Index < InputBindingClasses.Num(); ++Index)
        {
            UInputDelegateBinding* BindingObject = CastChecked<UInputDelegateBinding>(UBlueprintGeneratedClass::GetDynamicBindingObject(InClass, InputBindingClasses[Index]), ECastCheckedType::NullAllowed);
            if (!BindingObject)
                continue;

            if (UInputKeyDelegateBinding* KeyBinding = dynamic_cast<UInputKeyDelegateBinding*>(BindingObject))
            {
                TArray<FInputKeyBinding> BindsToAdd;

                for (int32 BindIndex = 0; BindIndex < KeyBinding->InputKeyDelegateBindings.Num(); ++BindIndex)
                {
                    const FBlueprintInputKeyDelegateBinding& Binding = KeyBinding->InputKeyDelegateBindings[BindIndex];
                    FInputKeyBinding KB(Binding.InputChord, Binding.InputKeyEvent);
                    KB.bConsumeInput = Binding.bConsumeInput;
                    KB.bExecuteWhenPaused = Binding.bExecuteWhenPaused;

                    KB.KeyDelegate.BindDelegate(GraphObject, Binding.FunctionNameToBind);

                    if (Binding.bOverrideParentBinding)
                    {
                        for (int32 ExistingIndex = InputComponent->KeyBindings.Num() - 1; ExistingIndex >= 0; --ExistingIndex)
                        {
                            const FInputKeyBinding& ExistingBind = InputComponent->KeyBindings[ExistingIndex];
                            if (ExistingBind.Chord == KB.Chord && ExistingBind.KeyEvent == KB.KeyEvent)
                            {
                                InputComponent->KeyBindings.RemoveAt(ExistingIndex);
                            }
                        }
                    }
                    BindsToAdd.Add(KB);
                }

                for (int32 Keyindex = 0; Keyindex < BindsToAdd.Num(); ++Keyindex)
                {
                    InputComponent->KeyBindings.Add(BindsToAdd[Keyindex]);
                }
            }
            else if (UInputActionDelegateBinding* ActionBinding = dynamic_cast<UInputActionDelegateBinding*>(BindingObject))
            {
                TArray<FInputActionBinding> BindsToAdd;

                for (int32 BindIndex = 0; BindIndex < ActionBinding->InputActionDelegateBindings.Num(); ++BindIndex)
                {
                    const FBlueprintInputActionDelegateBinding& Binding = ActionBinding->InputActionDelegateBindings[BindIndex];

                    FInputActionBinding AB(Binding.InputActionName, Binding.InputKeyEvent);
                    AB.bConsumeInput = Binding.bConsumeInput;
                    AB.bExecuteWhenPaused = Binding.bExecuteWhenPaused;

                    AB.ActionDelegate.BindDelegate(GraphObject, Binding.FunctionNameToBind);

                    if (Binding.bOverrideParentBinding)
                    {
                        for (int32 ExistingIndex = InputComponent->GetNumActionBindings() - 1; ExistingIndex >= 0; --ExistingIndex)
                        {
                            const FInputActionBinding& ExistingBind = InputComponent->GetActionBinding(ExistingIndex);
                            if (ExistingBind.GetActionName() == AB.GetActionName() && ExistingBind.KeyEvent == AB.KeyEvent)
                            {
                                InputComponent->RemoveActionBinding(ExistingIndex);
                            }
                        }
                    }

                    BindsToAdd.Add(AB);
                }

                for (int32 ActionIndex = 0; ActionIndex < BindsToAdd.Num(); ++ActionIndex)
                {
                    InputComponent->AddActionBinding(BindsToAdd[ActionIndex]);
                }
            }
        }
    }
}
